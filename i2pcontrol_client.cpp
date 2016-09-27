#include "i2pcontrol_client.h"

#include <sstream>
#include <iomanip>
#include <iostream>

#include <QtNetwork/QtNetwork>

I2PControlRequest::I2PControlRequest(const std::string& version)
    : id(), version(version), method(), params() {}

std::string I2PControlRequest::toJsonString() const
{
  std::ostringstream oss;
  oss << "{\"id\":\"" << id
      << "\",\"method\":\"" << method
      << "\",\"params\":{";
  for (auto it = params.begin(); it != params.end(); ++it) {
    if (it != params.begin())
      oss << ',';
    oss << '"' << it->first << "\":" << it->second;
  }
  oss << "},\"jsonrpc\":\"" << version << "\"}";
  return oss.str();
}

void I2PControlRequest::setParam(
    const std::string& param,
    const std::string& value)
{
    params[param] = value.empty() ? "null" : "\"" + value + "\"";
}

void I2PControlRequest::setParam(
    const std::string& param,
    int value)
{
    params[param] = std::to_string(value);
}

void I2PControlRequest::setParam(
    const std::string& param,
    double value)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    params[param] = oss.str();
}

void I2PControlRequest::setID(const std::string& identifier)
{
    id = identifier;
}

void I2PControlRequest::setMethod(const std::string& m)
{
    method = m;
}

I2PControlClient::I2PControlClient(const std::string& address)
    : serverAddress(address), token(), url(QString::fromStdString(address)),
      networkAccess()
{

}

void I2PControlClient::start(std::function<void(bool)> callback) {
    authenticate(callback);
}

void I2PControlClient::sendRequest(const I2PControlRequest& request,
                                   std::function<void(const QJsonDocument&)> callback)
{
    QNetworkRequest qRequest(url);
    qRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray data;
    data.append(QString::fromStdString(request.toJsonString()));
    std::cout << request.toJsonString() << std::endl;
    reply = networkAccess.post(qRequest, data);
    // Note: we must copy the callback and request to avoid destruction
    QObject::connect(reply, &QNetworkReply::finished, [&, callback, request]() {
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        const QJsonObject obj = document.object();
        if(obj.contains("error") && obj["error"].toObject()["code"].toInt() == -32003) {
            // Reauthenticate and then retry
            authenticate([this, request,  callback](bool success) {
                if(!success)
                    return; // Silently abort
                // Change the token and resend the request
                I2PControlRequest newRequest = request;
                newRequest.setParam("Token", token);
                sendRequest(newRequest, callback);
            });
        } else if (obj.contains("result")) {
            callback(document);
        }
    });
}

std::string I2PControlClient::getToken() const {
    return token;
}

void I2PControlClient::authenticate(std::function<void(bool)> callback) {
    I2PControlRequest request;
    request.setID("0");
    request.setMethod("Authenticate");
    request.setParam("API", "1");
    request.setParam("Password", "itoopie");

    sendRequest(request, [this, callback](const QJsonDocument& response) {
        // Store token
        QJsonObject responseObj = response.object();
        token = responseObj["result"].toObject()["Token"].toString().toStdString();
        std::cout << "Authenticated with token " << token << std::endl;
        callback(!token.empty());
    });
}
