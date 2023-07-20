#include <zmq.hpp>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        std::cout << "Please provide a message as a command line argument." << std::endl;
        return -1;
    }

    // Prepare the context and the socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);

    // Connect to the receiver's socket
    socket.connect("ipc:///tmp/test.sock");

    const std::string messageToSend = argv[1];

    // Send the message
    zmq::message_t message(messageToSend.size());
    memcpy(message.data(), messageToSend.data(), messageToSend.size());
    socket.send(message, zmq::send_flags::none);

    // Receive the reply
    zmq::message_t reply;
    zmq::recv_result_t result = socket.recv(reply, zmq::recv_flags::none);
    if (!result.has_value())
    {
        std::cout << "Failed to receive the reply." << std::endl;
        return -1;
    }

    std::string replyMessage(static_cast<char *>(reply.data()), reply.size());
    std::cout << "Received reply: " << replyMessage << std::endl;

    return 0;
}
