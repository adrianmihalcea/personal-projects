package com.pweb.backend.services;

import com.pweb.backend.dtos.LocationRequestsDTO;
import com.pweb.backend.dtos.RequestDTO;
import com.pweb.backend.dtos.SimpleResponseDTO;
import com.pweb.backend.entities.RequestEntity;
import com.pweb.backend.entities.LocationEntity;
import com.pweb.backend.repositories.RequestsRepository;
import com.pweb.backend.repositories.LocationsRepository;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.*;
import java.util.function.Function;
import java.util.stream.Collectors;

@Service
@Slf4j
public class RequestsService {
    private final RequestsRepository requestsRepository;
    private final LocationsRepository locationsRepository;
    private final AdminsService adminsService;
    private final MQService mqService;

    @Autowired
    public RequestsService(RequestsRepository requestsRepository,
                           LocationsRepository locationsRepository,
                           AdminsService adminsService,
                           MQService mqService) {
        this.requestsRepository = requestsRepository;
        this.locationsRepository = locationsRepository;
        this.adminsService = adminsService;
        this.mqService = mqService;

        if (requestsRepository.findAll().isEmpty()) {
            requestsRepository.saveAll(Arrays.asList(
                    new RequestEntity(1, "Item 1", 10, 0),
                    new RequestEntity(1, "Item 2", 15, 0),
                    new RequestEntity(2, "Item 1", 20, 0),
                    new RequestEntity(2, "Item 2", 25, 0)
            ));
        }
    }

    public LocationRequestsDTO getRequestsByLocation(int locationId, String user, String role) {
        if (role.equalsIgnoreCase("admin") && !adminsService.isAdmin(user, locationId)) {
            log.error(user + "is not an admin for location " + locationId);
            return new LocationRequestsDTO(locationId, new ArrayList<>());
        }

        List<RequestEntity> temp = requestsRepository.findAllByLocationIdOrderByPriorityDesc(locationId);
        if (Objects.isNull(temp) || temp.isEmpty()) {
            log.info("No requests for location " + locationId);
            return new LocationRequestsDTO(locationId, new ArrayList<>());
        }

        LocationRequestsDTO ret = new LocationRequestsDTO(locationId, temp);
        log.info("Requests for location " + locationId  + ": "
                + ret.getRequests().stream().map(RequestDTO::getName).collect(Collectors.joining(", ")));
        return ret;
    }

    public SimpleResponseDTO saveNewRequest(int locationId, RequestDTO request, String user) {
        if (!adminsService.isAdmin(user, locationId)) {
            return new SimpleResponseDTO(
                    "User " + user + " is not an admin for location " + locationId,
                    401
            );
        }

        RequestEntity oldRequest = requestsRepository.findByLocationIdAndItemName(locationId, request.getName());
        if (Objects.nonNull(oldRequest)) {
            log.info("Existing request for " + locationId + " found");
            request.setCount(request.getCount() + oldRequest.getCount());
            if (oldRequest.getPriority() > request.getPriority()) {
                request.setPriority(oldRequest.getPriority());
            }
        }

        LocationEntity locationDetails = locationsRepository.findLocationDetailsEntityById(locationId);
        if (Objects.isNull(locationDetails)) {
            log.info("There is no location with id = " + locationId);
            return new SimpleResponseDTO("There is no location with id = " + locationId, 400);
        }

        RequestEntity toSave = new RequestEntity(locationId, request.getName(), request.getCount(), request.getPriority());
        log.info("Saving: " + request + " for location " + locationId);
        requestsRepository.save(toSave);

        return new SimpleResponseDTO("", 200);
    }

    public SimpleResponseDTO markReceived(int locationId, List<RequestDTO> fulfilledRequests, String user) {
        List<RequestEntity> filteredItemLocationList = requestsRepository.findAllByLocationIdAndItemNameIsIn(
                locationId,
                fulfilledRequests.stream().map(RequestDTO::getName).collect(Collectors.toList()));

        Map<String, RequestDTO> affectedRequests = new LocationRequestsDTO(locationId, filteredItemLocationList)
                .getRequests().stream().collect(Collectors.toMap(
                        RequestDTO::getName,
                        Function.identity()
                ));

        List<RequestEntity> toSave = new ArrayList<>();
        List<RequestEntity> toDelete = new ArrayList<>();

        for (RequestDTO request : fulfilledRequests) {
            if (!affectedRequests.containsKey(request.getName())) {
                log.info("Location with id=" + locationId + " has no request for " + request.getName());
                return new SimpleResponseDTO("Location with id=" + locationId + " has no request for " + request.getName(), 400);
            }

            RequestDTO current = affectedRequests.get(request.getName());
            if (request.getCount() >= current.getCount()) {
                current.setCount(0);
                toDelete.add(current.getItemLocationEntity(locationId));

                log.info("Deleting request for item=" + current.getName() + " (location=" + locationId + ")");
            } else {
                current.setCount(current.getCount() - request.getCount());
                toSave.add(current.getItemLocationEntity(locationId));

                log.info("Updating count for item=" + current.getName() + " (location=" + locationId + ")");
            }
        }

        if (!toSave.isEmpty()) {
            requestsRepository.saveAll(toSave);
        }

        if (!toDelete.isEmpty()) {
            requestsRepository.deleteAll(toDelete);
        }

        mqService.sendToBroker(locationId, fulfilledRequests.stream()
                .filter(req -> req.getCount() != 0)
                .collect(Collectors.toList()), user);

        return new SimpleResponseDTO("", 200);
    }
}
