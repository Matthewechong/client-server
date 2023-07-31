#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <thread>
using namespace std;
using boost::asio::ip::udp;

class server
{
public:
  server(boost::asio::io_context& io_context, short port)
    : socket_(io_context, udp::endpoint(udp::v4(), port))
  {
    do_receive();
  }

  void do_receive()
  {
    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        [this](boost::system::error_code ec, std::size_t bytes_recvd)
        {
          if (!ec && bytes_recvd > 0)
          {
            std::thread recieve_thread(&server::do_send,this,bytes_recvd);
            recieve_thread.join();
            // do_send(bytes_recvd);
          }
          else
          {
            do_receive();
          }
        });
  }

  void do_send(std::size_t length)
  {
    cout << "Listening..." << endl;
    cout << "Recieved from client: " << endl;
    cout << data_ << endl;
    cout << "Send message to client: " << endl;
    
    // Automated Response
    // char data[] = "Hello from the server!!!";
    // std::size_t d_length = std::strlen(data);
    // message(data,d_length);
    cout << "Listening for response..." << endl;

    // Customized Response
    char res[max_length];
    std::cin.getline(res, max_length);
    std::size_t res_length = std::strlen(res);
    message(res,res_length);
  }

  void message(const char data[],std::size_t d_length){
    socket_.async_send_to(
        boost::asio::buffer(data, d_length), sender_endpoint_,
        [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/)
        {
          do_receive();
        });
  }

private:
  udp::socket socket_;
  udp::endpoint sender_endpoint_;
  enum { max_length = 1024 };
  char data_[max_length];
  
};

int main(int argc, char* argv[])
{
  try
  {

    boost::asio::io_context io_context;

    server s(io_context, std::atoi("9090"));
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}