#include <boost/asio.hpp>
#include <atomic>
#include <memory>
#include <functional>
#include <spdlog/spdlog.h>

namespace AsioCalc {
class Acceptor {
public:
    Acceptor(boost::asio::io_service&ios, int port_num,
             std::function<std::string(std::istream&)> processRequest);

    // Start accepting incoming connection requests.
    void start();
    // Stop accepting incoming connection requests.
    void stop();
private:
    void initAccept();
    void onAccept(const boost::system::error_code &ec,
                  std::shared_ptr<boost::asio::ip::tcp::socket> sock);
private:
    boost::asio::io_service& mIoService;
    boost::asio::ip::tcp::acceptor mAcceptor;
    std::atomic<bool> mIsStopped{false};
    std::function<std::string(std::istream&)> mProcessRequest;
    std::shared_ptr<spdlog::logger> logger;
};

class Server {
public:
    Server(int portNum,
           boost::asio::io_service &m_ios,
           std::function<std::string(std::istream&)> processRequest);
    void Start();
    void Stop();
private:
    std::shared_ptr<spdlog::logger> mLogger{spdlog::stdout_color_mt("server")};
    boost::asio::io_service::work mWork;
    Acceptor mAcceptor;
};
}
