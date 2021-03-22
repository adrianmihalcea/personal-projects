package com.covid.codelorians;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableScheduling;

@SpringBootApplication
@EnableScheduling
public class CodeloriansApplication {

    public static void main(String[] args) {
        SpringApplication.run(CodeloriansApplication.class, args);
    }

}
