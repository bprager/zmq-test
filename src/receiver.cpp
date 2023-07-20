#include <zmq.hpp>
#include <string>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>

int main()
{
    // Prepare the context and the socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP); // REP stands for REPlies. This socket type is used to reply to incoming requests.

    try
    {
        socket.bind("ipc:///tmp/test.sock");
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error binding the socket: " << ex.what() << std::endl;
        return 1;
    }

    std::cout << "Bound to socket. Waiting for the sender to connect and send a message..." << std::endl;

    while (true)
    {
        zmq::message_t request;

        // Check if there are any pending messages
        zmq::pollitem_t items[] = {{socket, 0, ZMQ_POLLIN, 0}};
        zmq::poll(&items[0], 1, std::chrono::milliseconds(100)); // Wait for 100 milliseconds

        if (items[0].revents & ZMQ_POLLIN)
        {
            // Message received, proceed with recv
            zmq::recv_result_t recvResult;
            try
            {
                recvResult = socket.recv(request, zmq::recv_flags::none);
            }
            catch (const std::exception &ex)
            {
                std::cerr << "Error receiving message: " << ex.what() << std::endl;
                return 1;
            }

            // Use the recvResult variable to avoid the warning
            (void)recvResult;

            std::string replyMessage = std::string(static_cast<char *>(request.data()), request.size());

            // Print the message
            std::cout << "Received message: " << replyMessage << std::endl;

            // Send reply back to the client
            zmq::message_t reply(3);
            memcpy(reply.data(), "ACK", 3);

            try
            {
                socket.send(reply, zmq::send_flags::none);
            }
            catch (const std::exception &ex)
            {
                std::cerr << "Error sending reply: " << ex.what() << std::endl;
                return 1;
            }

            // If the message is "exit", then break the loop and end the program
            if (replyMessage == "exit")
            {
                break;
            }
        }
        else
        {
            // No message received, sleep for a short duration to avoid busy-waiting
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    return 0;
}
