#include "HelloWorldConsumer.h"
#include <iostream>

void HelloWorldConsumer::run()
{
	try
	{
		auto_ptr<ConnectionFactory> connectionFactory(ConnectionFactory::createCMSConnectionFactory(brokerURI));

		connection = connectionFactory->createConnection();
		connection->start();
		connection->setExceptionListener(this);

		if (sessionTransacted)
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

		consumer = session->createConsumer(destination);

		consumer->setMessageListener(this);

		std::cout.flush();
		std::cerr.flush();

		latch.countDown();

		doneLatch.await(waitMillis);
	}
	catch (cms::CMSException &e)
	{
		latch.countDown();
		e.printStackTrace();
	}
}

void HelloWorldConsumer::onMessage(const Message *message)
{
	static int count = 0;

	try
	{
		count++;

		const TextMessage *textMessage = dynamic_cast<const TextMessage *>(message);

		string text = "";

		if (textMessage == nullptr)
		{
			text = "NOT A TEXTMASSAGE!";
		}
		else
		{
			text = textMessage->getText();
		}

		cout << "Message " << count << " Recieve " << text << endl;
	}
	catch (CMSException &e)
	{
		e.printStackTrace();
	}

	if (this->sessionTransacted)
	{
		session->commit();
	}

	doneLatch.countDown();
}

void HelloWorldConsumer::onException(const CMSException &ex AMQCPP_UNUSED)
{
	cout << "CMS Exception occurred.  Shutting down client.\n";
	ex.printStackTrace();
	exit(1);
}

void HelloWorldConsumer::cleanup()
{
	if (connection != nullptr)
	{
		try
		{
			connection->close();
		}
		catch (CMSException &e)
		{
			e.printStackTrace();
		}
	}

	try
	{
		delete destination;
		destination = nullptr;

		delete consumer;
		consumer = nullptr;

		delete session;
		session = nullptr;

		delete connection;
		connection = nullptr;
	}
	catch (cms::CMSException &e)
	{
		e.printStackTrace();
	}
}
