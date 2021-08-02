# Paho C MQTT Client Example

## Overview
This is an MQTT client example project that showcases how you can use HiveMQ Cloud with the Eclipse Paho C Client. The example project covers the basic MQTT functionality: Connecting MQTT clients to your HiveMQ Cloud cluster, subscribing to topics and publishing data (sending and receiving messages using the MQTT protocol).

The Eclipse Paho project provides open source, mainly client side, implementations of MQTT and MQTT-SN in a variety of programming languages.
The Python client used here supports MQTT v3.1, v3.1.1 and v5.
You can find documentation for this client library here: https://www.hivemq.com/blog/mqtt-client-library-pahocclient/.

This example repository is easily and clearly structured, so you can start quickly:
This readme file is your starting point. Here we will describe what you have to do step-by-step to get started with this example.
[``main.c``](main.c) is a simple implementation that demonstrates the core functionality of an MQTT Client.
Follow the instructions in the following paragraphs to get started yourself.

## HiveMQ Cloud
[HiveMQ](https://www.hivemq.com/) is the industry leader for enterprise-ready, beautifully scalable, large-scale IoT deployments with MQTT. We help companies connect things to the Internet. Our MQTT-based messaging platform ensures fast, reliable, and secure movement of data to and from connected IoT devices for companies around the world. HiveMQ Cloud is our fully-managed, cloud-native IoT messaging platform that makes trustworthy and scalable IoT device connectivity simple. You can learn more about HiveMQ Cloud on our [website](https://www.hivemq.com/mqtt-cloud-broker/), [documentation](https://www.hivemq.com/docs/hivemq-cloud/introduction.html)  and our [blog posts](https://www.hivemq.com/tags/cloud/).

## Getting started
[By signing up](https://console.hivemq.cloud) for HiveMQ Cloud you will automatically get access to a HiveMQ Cloud Basic cluster. HiveMQ Cloud Basic is our smallest package that allows you to connect up to 100 MQTT clients for free and test the full MQTT functionality. 

The [HiveMQ Cloud Quick Start Guide](https://www.hivemq.com/docs/hivemq-cloud/introduction.html#guide) gives you step-by-step instructions on how to set up your HiveMQ Cloud account, create clusters, and connect MQTT clients.


### Prerequisites 
After signing up, you have a running HiveMQ Cloud cluster, that you can use in this example.
Now clone this repository into your local IDE.

For using the code examples, you need to install [OpenSSL](https://www.openssl.org/).
```
git clone https://github.com/openssl/openssl.git
cd openssl
./Configure
make
make test
cd ..
```
Also install the paho mqtt c library, the client library used here.
```
git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c
make
sudo make install
cd ..
```

### Broker credentials
To define the HiveMQ Cloud cluster which should be targeted, you need to fill the placeholders in the code with your host name, username and password.
The <b>host name</b> can be found in the <b>Details</b> section of the <b>Overview</b> tab of your cluster.
![cluster overview](/img/hivemq-cloud-cluster-overview.png)

After the cluster is created, add a set of credentials that you can use in this example or future implementations.
Use any secure username and password you desire.
The <b>username</b> and <b>password</b> are the values used as <b>Client Credentials</b> that you just created.
![credentials](/img/hivemq-cloud-credentials.png)

### Code Examples
This example project covers the core functionality of an MQTT client interacting with HiveMQ Cloud.
To securely connect the MQTT client with HiveMQ Cloud you need to use TLS.
Use your username and password, to authenticate your MQTT client at HiveMQ Cloud.
To connect the client, use the port 8883 that is standard for secure MQTT communication. 

```C
// the serverURI has to be in the format "protocol://name:port", in this case it should be "ssl://name:8883"
MQTTClient_create(&client, "<your_host_name>", "<your_client_id>", MQTTCLIENT_PERSISTENCE_NONE, NULL);
```
```C
conn_opts.username = "<your_username>";
conn_opts.password = "<your_password>";
```

The code located inside [``main.c``](main.c) connects to the configured HiveMQ Cloud Broker in a simple way. 
This is a ready-set example that can simply be run after inputting your credentials with:
```sh
gcc main.c -l paho-mqtt3cs -o main.o
./main.o
```

The code first subscribes to the topic filter "<your_topic>".  
That means the MQTT client receives all messages that are published to this [topic filter](https://www.hivemq.com/blog/mqtt-essentials-part-5-mqtt-topics-best-practices/).
It prints a reassurance that the subsription worked.
Then the code publishes a message with the content (payload) specified in ``"<your_payload>"`` to the same topic.
```C
// the topic where you publish to and subscribe to
const char* topic = "<your_topic>";
MQTTClient_subscribe(client, topic, qos);
printf("subscribed to %s \n", topic);

// payload the content of your message
char* payload = "<your_payload>";
int payloadlen = strlen(payload);
MQTTClient_deliveryToken dt;
MQTTClient_publish(client, topic, payloadlen, payload, qos, retained, &dt);
printf("published to %s \n", topic);
```

Once the message is received, the callback ``messageArrived`` prints a confirmation and the content of the message.
```C
int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    printf("Message arrived %s \n", (char*)message->payload);
    return 1;
}
```

## Learn more
If you want to learn more about MQTT, visit the [MQTT Essentials](https://www.hivemq.com/mqtt-essentials/) guide, 
that explains the core of MQTT concepts, its features and other essential information. To learn more about MQTT on Paho C,
visit the [official website](https://www.eclipse.org/paho/files/mqttdoc/MQTTClient/html/index.html) of the Eclipse Foundation 
and check out their [Github repository](https://github.com/eclipse/paho.mqtt.c).

## Contributing

Please see our [contributing guidelines](./CONTRIBUTING.adoc) and [code of conduct](./code-of-conduct.md).

## License

[Apache 2.0](./LICENSE).
