#ifndef I2PCONTROLCLIENT_H
#define I2PCONTROLCLIENT_H

#include <string>
#include <array>
#include <functional>

#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>

/**
 * @brief Class representing the JSON content of an I2PControl request.
 */
class I2PControlRequest {
public:
    /**
     * @brief I2PControlRequest constructor
     * @param version the JSONRPC version, by default 2.0
     */
    I2PControlRequest(const std::string& version = "2.0");

    /**
     * @brief Creates a string repesentation of the JSON content.
     * @return the string representation of the JSON data
     */
    std::string toJsonString() const;

    /**
     * @brief Sets a request parameter to a specified string.
     * @param param the name of the parameter to be set
     * @param value the value of the parameter
     * @note Overrides existing values.
     * @todo escape quotes (solve by using ::QJsonDocument)
     */
    void setParam(const std::string& param, const std::string& value);

    /**
     * @brief Sets a request parameter to a specified integer.
     * @param param the name of the parameter to be set
     * @param value the value of the parameter
     * @note Overrides existing values.
     */
    void setParam(const std::string& param, int value);

    /**
     * @brief Sets a request parameter to a specified double.
     * @param param the name of the parameter to be set
     * @param value the value of the parameter
     * @note Overrides existing values.
     */
    void setParam(const std::string& param, double value);

    /**
     * @brief Sets the JSONRPC id to a given string.
     * @param id the new identifier
     */
    void setID(const std::string& id);

    /**
     * @brief Sets the I2PControl method to a given string.
     * @param method the name of the method
     */
    void setMethod(const std::string& method);

private:
    std::string id;
    std::string version;
    std::string method;
    std::map<std::string, std::string> params;
};

class QNetworkReply;

/**
 * @brief Provides functiality to communicate with an I2PControl server over HTTP.
 */
class I2PControlClient {
public:
    /**
     * @brief I2PControlClient constructor
     * @param url the location of the HTTP document providing the JSONRPC API.
     */
    I2PControlClient(const std::string& url);

    /**
     * @brief Starts the ::I2PControlClient.
     * @param callback the function to be called when the client has been started
     */
    void start(std::function<void(bool)> callback);

    /**
     * @brief Sends an ::I2PControlRequest to the I2PControl server.
     * @param request the request to be sent
     * @param callback the function to be called when the request has finished
     */
    void sendRequest(const I2PControlRequest& request,
                     std::function<void(const QJsonDocument&)> callback);

    /**
     * @brief Gets the authenticated token required in most I2PControl messages.
     * @return the authentication token
     */
    std::string getToken() const;
private:

    /**
     * @brief Authenticates the I2PControl client to the server.
     * @param callback the callback to be called after authentication has completed.
     */
    void authenticate(std::function<void(bool)> callback);

    std::string serverAddress, token;
    QUrl url;
    QNetworkReply* reply;
    QNetworkAccessManager networkAccess;
};

#endif // I2PCONTROLCLIENT_H
