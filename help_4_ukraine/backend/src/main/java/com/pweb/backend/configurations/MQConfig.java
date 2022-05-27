package com.pweb.backend.configurations;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Configuration;

@Configuration
public class MQConfig {
    @Value("${mqtt.broker}")
    private String broker;

    public String getBroker() {
        return broker;
    }
}
