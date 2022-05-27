package com.pweb.backend.services;

import com.pweb.backend.entities.LocationEntity;
import com.pweb.backend.repositories.LocationsRepository;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.Arrays;
import java.util.List;

@Service
@Slf4j
public class LocationsService {
    private final LocationsRepository locationsRepository;

    @Autowired
    public LocationsService(LocationsRepository locationsRepository) {
        this.locationsRepository = locationsRepository;

        if (locationsRepository.findAll().isEmpty()) {
            locationsRepository.saveAll(Arrays.asList(
                    new LocationEntity(0, "1st Location", "1st Address"),
                    new LocationEntity(0, "2nd Location", "2nd Address")
            ));
        }
    }

    public List<LocationEntity> getAllLocationDetails() {
        List<LocationEntity> temp = locationsRepository.findAll();

        log.info(temp.toString());

        return temp;
    }
}
