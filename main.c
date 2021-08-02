/*
 * Copyright 2021 HiveMQ GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include "MQTTClient.h"
#include "paho.mqtt.c/src/samples/pubsub_opts.h"

int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    printf("Message arrived %s \n", (char*)message->payload);
    return 1;
}

int main(void) {
    int rc = 1; // return code
    int qos = 1;
    int retained = 0;
    MQTTClient client;
    // the serverURI has to be in the format "protocol://name:port", in this case it should be "ssl://name:8883"
    MQTTClient_create(&client, "<your_host_name>", "<your_client_id>", MQTTCLIENT_PERSISTENCE_NONE, NULL);

    // you can set optional callbacks for context, connectionLost, messageArrived and deliveryComplete
    int i = MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);
    printf("callback: %d \n", i); // callback 0 signalizes a successful connection

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
    ssl_opts.enableServerCertAuth = 0;

    // declare values for ssl options, here we use only the ones necessary for TLS, but you can optionally define a lot more
    // look here for an example: https://github.com/eclipse/paho.mqtt.c/blob/master/src/samples/paho_c_sub.c
    ssl_opts.verify = 1;
    ssl_opts.CApath = NULL;
    ssl_opts.keyStore = NULL;
    ssl_opts.trustStore = NULL;
    ssl_opts.privateKey = NULL;
    ssl_opts.privateKeyPassword = NULL;
    ssl_opts.enabledCipherSuites = NULL;

    // use TLS for a secure connection, "ssl_opts" includes TLS
    conn_opts.ssl = &ssl_opts;
    conn_opts.keepAliveInterval = 10;
    conn_opts.cleansession = 1;
    // use your credentials that you created with the cluster
    conn_opts.username = "<your_username>";
    conn_opts.password = "<your_password>";
    int j = MQTTClient_connect(client, &conn_opts);
    printf("is connected %d \n", j);

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

    // after this time, unsubscribe, disconnect and destroy the client to terminate every process
    int timeout = 100;
    MQTTClient_unsubscribe(client, topic);
    MQTTClient_disconnect(client, timeout);
    MQTTClient_destroy(&client);

    return rc;
}
