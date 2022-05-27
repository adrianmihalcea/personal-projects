package com.pweb.backend.services;

import com.pweb.backend.dtos.DonationDTO;
import com.pweb.backend.dtos.RequestDTO;
import com.pweb.backend.dtos.SimpleResponseDTO;
import com.pweb.backend.entities.DonationHistoryEntity;
import com.pweb.backend.entities.LocationEntity;
import com.pweb.backend.repositories.DonationHistoryRepository;
import com.pweb.backend.repositories.LocationsRepository;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.sql.Timestamp;
import java.time.Instant;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

@Service
@Slf4j
public class DonationHistoryService {
    private final DonationHistoryRepository donationHistoryRepository;
    private final LocationsRepository locationsRepository;
    private final AdminsService adminsService;

    @Autowired
    public DonationHistoryService(DonationHistoryRepository donationHistoryRepository,
                                  LocationsRepository locationsRepository, AdminsService adminsService) {
        this.donationHistoryRepository = donationHistoryRepository;
        this.locationsRepository = locationsRepository;
        this.adminsService = adminsService;

        if (donationHistoryRepository.findAll().isEmpty()) {
            donationHistoryRepository.saveAll(Arrays.asList(
                    new DonationHistoryEntity(1, "Item 1", 1, "test@user.com", Timestamp.from(Instant.now())),
                    new DonationHistoryEntity(1, "Item 2", 2, "test@user.com", Timestamp.from(Instant.now())),
                    new DonationHistoryEntity(2, "Item 1", 3, "test@user.com", Timestamp.from(Instant.now()))
            ));
        }
    }

    public SimpleResponseDTO addToHistory(int locationId, List<RequestDTO> fulfilledRequests, String user) {
        donationHistoryRepository.saveAll(fulfilledRequests.stream().map(requestDTO ->
                        new DonationHistoryEntity(locationId, requestDTO.getName(), requestDTO.getCount(), user, Timestamp.from(Instant.now())))
                .collect(Collectors.toList()));
        return new SimpleResponseDTO("", 200);
    }

    public List<DonationDTO> getHistoryForLocation(int locationId, String user) {
        if (!adminsService.isAdmin(user, locationId)) {
            return new ArrayList<>();
        }

        LocationEntity locationDetails = locationsRepository.findLocationDetailsEntityById(locationId);
        if (Objects.isNull(locationDetails)) {
            log.info("There is no location with id = " + locationId);
            return null;
        }

        return donationHistoryRepository.findAllByLocationIdOrderByDonatedByDesc(locationId).stream()
                .map(DonationDTO::new)
                .collect(Collectors.toList());
    }
}
