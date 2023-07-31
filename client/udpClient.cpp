#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <thread>
using namespace std;
using boost::asio::ip::udp;

enum { max_length = 1024 };


class client
{
  private:
    udp::socket socket_; // udp::socket as a member variable
    udp::resolver resolver;
    udp::resolver::results_type endpoints;
  public:
    client( boost::asio::io_context& io_context, const string& ip_address, const string& port) 
    : socket_(io_context, udp::endpoint(udp::v4(), 0)), 
    resolver(io_context){

      this->endpoints = resolver.resolve(udp::v4(), ip_address, port);

    }
    udp::socket& getSocket(){
      return socket_;
    }

    udp::resolver::results_type& getEndpoints(){
      return endpoints;
    }

    void recieveMessage(){
      char reply[max_length];
      udp::endpoint sender_endpoint;
      size_t reply_length = getSocket().receive_from(boost::asio::buffer(reply, max_length), sender_endpoint);
      std::cout << "Reply is: " << std::endl;
      std::cout.write(reply, reply_length);
      std::cout << "\n";
    }


};

int main(int argc, char* argv[])
{
  try
  {
    const string IP_ADDRESS = "127.0.0.1";
    const string PORT = "9090";
    boost::asio::io_context io_context;
    client client_socket(io_context,IP_ADDRESS, PORT);
    udp::resolver::results_type endpoints =client_socket.getEndpoints();


    // Sending message to server
    std::cout << "Enter message: ";
    char request[max_length];
    std::cin.getline(request, max_length);
    size_t request_length = std::strlen(request);
    client_socket.getSocket().send_to(boost::asio::buffer(request, request_length), *endpoints.begin());


    // Recieving message from server
    // std::thread listener([&client_socket]() { client_socket.recieveMessage(); });
    client_socket.recieveMessage();
    
    
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}