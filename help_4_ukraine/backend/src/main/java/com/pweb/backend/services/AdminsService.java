package com.pweb.backend.services;

import com.pweb.backend.entities.LocationAdminEntity;
import com.pweb.backend.repositories.AdminsRepository;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.oauth2.jwt.Jwt;
import org.springframework.stereotype.Service;

import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

@Service
@Slf4j
public class AdminsService {
    private final AdminsRepository adminsRepository;

    @Autowired
    public AdminsService(AdminsRepository adminsRepository) {
        this.adminsRepository = adminsRepository;

        if (adminsRepository.findAll().isEmpty()) {
            adminsRepository.saveAll(Arrays.asList(
                    new LocationAdminEntity(1, "amihai.mihalcea@gmail.com"),
                    new LocationAdminEntity(1, "ursu.andrei1999@gmail.com")
            ));
        }
    }

    public List<String> getAdminEmailsForLocation(int locationId) {
        return adminsRepository.findLocationAdminEntitiesByLocationId(locationId)
                .stream()
                .map(LocationAdminEntity::getEmail)
                .collect(Collectors.toList());
    }

    public boolean isAdmin(String email, int locationId) {
        return adminsRepository.existsByEmailAndLocationId(email, locationId);
    }

    public static String getEmailFromSecurityContext() {
        try {
            return ((Jwt) SecurityContextHolder.getContext().getAuthentication().getPrincipal())
                    .getClaimAsString("https://test.com/email");
        } catch (Exception e) {
            log.error("Couldn't get user email: " + e.getMessage());
            return "n/a";
        }
    }
}
