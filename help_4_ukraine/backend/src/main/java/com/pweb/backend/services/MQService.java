package com.pweb.backend.services;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.pweb.backend.configurations.MQConfig;
import com.pweb.backend.dtos.RequestDTO;
import lombok.extern.slf4j.Slf4j;
import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.*;

@Service
@Slf4j
public class MQService {
    private final ObjectMapper objectMapper;
    private final AdminsService adminsService;
    private final IMqttClient publisher;

    @Autowired
    public MQService(AdminsService adminsService, MQConfig mqConfig) throws MqttException {
        this.adminsService = adminsService;
        objectMapper = new ObjectMapper();
        String broker = mqConfig.getBroker();

        if (Objects.isNull(broker) || broker.isEmpty()) {
            publisher = null;
            log.error("Couldn't connect to broker");
            return;
        }

        String publisherId = UUID.randomUUID().toString();
        publisher = new MqttClient(broker, publisherId);
        publisher.connect();
    }

    public boolean isConnected() {
        return publisher.isConnected();
    }

    private String getPayload(int locationId, RequestDTO fulfilledRequest, String user, List<String> admins) {
        Map<String, String> payload = new TreeMap<>();
        payload.put("user", user);
        payload.put("location", String.valueOf(locationId));
        payload.put("donation", fulfilledRequest.toString());
        payload.put("admin", String.join(",", admins));

        try {
            return objectMapper.writeValueAsString(payload);
        } catch (JsonProcessingException e) {
            log.error("Couldn't create MQTT message payload");
            return "";
        }
    }

    public void sendToBroker(int locationId, List<RequestDTO> fulfilledRequests, String user) {
        List<String> admins = adminsService.getAdminEmailsForLocation(locationId);
        fulfilledRequests.forEach(request ->
                {
                    MqttMessage message = new MqttMessage();
                    message.setQos(0);
                    message.setRetained(false);

                    String payload = getPayload(locationId, request, user, admins);
                    message.setPayload(payload.getBytes());
                    try {
                        log.info("Published on topic " + locationId + " payload: " + payload);
                        publisher.publish(String.valueOf(locationId), message);
                    } catch (MqttException e) {
                        log.error("Couldn't publish to message queue");
                    }
                });
    }
}
