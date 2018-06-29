#include <decaf/lang/Integer.h>
#include "HelloWorldConsumer.h"
#include "HelloWorldProducer.h"

using namespace decaf::lang;

int main(int argc AMQCPP_UNUSED, char *argv[] AMQCPP_UNUSED)
{
    activemq::library::ActiveMQCPP::initializeLibrary();
    {
        std::cout << "***************************************\n";
        std::cout << "Starting example:\n";
        std::cout << "---------------------------------------\n";

        std::string brokerURI = "failover:(tcp://127.0.0.1:61616)";

        bool useTopic = false;
        bool sessionTransacted = false;
        int numMessages = 2000;

        long long startTime = System::currentTimeMillis();

        HelloWorldProducer producer(brokerURI, numMessages, useTopic);
        HelloWorldConsumer consumer(brokerURI, numMessages, useTopic, sessionTransacted);

        Thread  consumerThread(&consumer);
        consumerThread.start();

        consumer.waitUntilReady();

        Thread producerThread(&producer);
        producerThread.start();

        producerThread.join();
        consumerThread.join();

        long long endTime = System::currentTimeMillis();
        double totalTime = double(endTime - startTime) / 1000.0;

        consumer.close();
        producer.close();

        std::cout << "Time ro completion: " << totalTime << " seconds\n";

        std::cout << "---------------------------------------\n";
        std::cout << "Finish with the example.\n";
    }
    activemq::library::ActiveMQCPP::shutdownLibrary();

    return 0;
}
