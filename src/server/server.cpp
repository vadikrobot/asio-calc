#include "server.h"
#include <spdlog/spdlog.h>

namespace AsioCalc {
Acceptor::Acceptor(boost::asio::io_service &ios, int port_num,
                   std::function<std::string(std::istream&)> processRequest):
    mIoService(ios),
    mAcceptor(mIoService,
              boost::asio::ip::tcp::endpoint(
                  boost::asio::ip::address_v4::any(),
                  port_num)),
    mProcessRequest(std::move(processRequest)) {
    logger = spdlog::get("server");
}

void Acceptor::start() {
    mAcceptor.listen();
    initAccept();
}

void Acceptor::stop() {
    mIsStopped = true;
}

void Acceptor::initAccept() {
    auto sock = std::make_shared<boost::asio::ip::tcp::socket> (mIoService);

    auto callBack = [this, sock](const boost::system::error_code& error) {
        onAccept(error, sock);
    };

    mAcceptor.async_accept(*sock.get(), callBack);
}

void Acceptor::onAccept(const boost::system::error_code &ec, std::shared_ptr<boost::asio::ip::tcp::socket> sock)
{
    logger->debug("new request");
    if (ec == 0) {
        auto request = std::make_shared<boost::asio::streambuf>();
        auto callBack = [this, request, sock] (const boost::system::error_code& ec, std::size_t bytes_transferred) {
            std::istream buffer(&*request);
            auto response = mProcessRequest(buffer);
            boost::asio::async_write(*sock.get(),
                                     boost::asio::buffer(response.c_str(), response.size()),
                                     [this, sock](const boost::system::error_code& ec, std::size_t bytes_transferred) {
                if (ec != 0) {
                    logger->error("Error occured during write! Error code = {}. Message {}", ec.value(), ec.message());
                }
                logger->debug("writing is completed");
            }); };
        boost::asio::async_read_until(*sock.get(), *request, '\0', callBack);
    } else {
        logger->error("Error occured! Error code = {}. Message {}", ec.value(), ec.message());
    }
    // Init next async accept operation if
    // acceptor has not been stopped yet.
    if (!mIsStopped) {
        initAccept();
    } else {
        // Stop accepting incoming connections
        mAcceptor.close();
    }
}

Server::Server(int portNum, boost::asio::io_service &io_service, std::function<std::string (std::istream &)> processRequest):
    mWork(boost::asio::io_service::work(io_service)),
    mAcceptor(io_service, portNum, processRequest) {
}

void Server::Start() {
    mAcceptor.start();
    mLogger->info("Server started");
}

void Server::Stop() {
    mAcceptor.stop();
    mLogger->info("Server stoped");
}
}
