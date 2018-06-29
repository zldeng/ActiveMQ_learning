#include "HelloWorldProducer.h"
#include <iostream>

void HelloWorldProducer::run()
{
	try
	{
		auto_ptr<ConnectionFactory> connectionFactory(ConnectionFactory::createCMSConnectionFactory(brokerURI));

		connection = connectionFactory->createConnection();
		connection->start();

		if (this->sessionTransacted)
		{
			session = connection->createSession(Session::SESSION_TRANSACTED);
		}
		else
		{
			session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
		}

		if (useTopic)
		{
			destination = session->createTopic("TEST.FOO");
		}
		else
		{
			destination = session->createQueue("TEST.FOO");
		}

		producer = session->createProducer(destination);
		producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);

		const string  threadIdStr = Long::toString(Thread::currentThread()->getId());

		string text = (string)("hello world")  + threadIdStr;

		for( auto idx = 0;idx < numMessages;idx++)
		{
			std::auto_ptr<TextMessage> message(session->createTextMessage(text));
			message->setIntProperty("Integer",idx);
			cout << "Send message " << idx << " from thread " << threadIdStr << endl;

			producer->send(message.get());
		}
	}
	catch (CMSException& e)
	{
		e.printStackTrace();
	}
}


void HelloWorldProducer::cleanup()
{
    
        if (connection != nullptr)
        {
            try
            {
                connection->close();
            }
            catch (cms::CMSException& e)
            {
                e.printStackTrace();
            }
        }

        try
        {
            delete destination;
            destination = nullptr;

            delete producer;
            producer = nullptr;

            delete session;
            session = nullptr;

            delete connection;
            connection = nullptr;

        }
        catch (CMSException& e)
        {
            e.printStackTrace();
        }
}
