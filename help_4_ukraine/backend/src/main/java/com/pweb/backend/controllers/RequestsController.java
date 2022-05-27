package com.pweb.backend.controllers;

import com.pweb.backend.dtos.SimpleResponseDTO;
import com.pweb.backend.dtos.LocationRequestsDTO;
import com.pweb.backend.dtos.RequestDTO;
import com.pweb.backend.services.AdminsService;
import com.pweb.backend.services.DonationHistoryService;
import com.pweb.backend.services.RequestsService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Objects;

@Transactional
@RestController
@CrossOrigin(origins = "*")
@RequestMapping("/api/requests")
public class RequestsController {
    private final RequestsService requestsService;
    private final DonationHistoryService donationHistoryService;

    @Autowired
    public RequestsController(RequestsService requestsService,
                              DonationHistoryService donationHistoryService) {
        this.requestsService = requestsService;
        this.donationHistoryService = donationHistoryService;
    }

    @GetMapping(value = "/view",
            produces = MediaType.APPLICATION_JSON_VALUE)
    public LocationRequestsDTO getRequestsForLocation(
            @RequestParam("locationId") int locationId,
            @RequestParam("role") String role
    ) {
        return requestsService.getRequestsByLocation(locationId, AdminsService.getEmailFromSecurityContext(), role);
    }

    @PostMapping(value = "/add",
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity<SimpleResponseDTO> addRequestForLocation(
            @RequestParam("locationId") int locationId,
            @RequestBody RequestDTO request
    ) {
        SimpleResponseDTO response = requestsService.saveNewRequest(locationId, request, AdminsService.getEmailFromSecurityContext());
        return ResponseEntity.status(response.getStatus()).body(response);
    }

    @PostMapping(value = "/received",
            consumes = MediaType.APPLICATION_JSON_VALUE,
            produces = MediaType.APPLICATION_JSON_VALUE
    )
    public ResponseEntity<SimpleResponseDTO> markReceivedItems(
            @RequestParam("locationId") int locationId,
            @RequestBody List<RequestDTO> fulfilledRequests
    ) {
        String user = AdminsService.getEmailFromSecurityContext();
        SimpleResponseDTO response = requestsService.markReceived(locationId, fulfilledRequests, user);

        if (response.getStatus() != 200) {
            return ResponseEntity.status(response.getStatus()).body(response);
        }

        // add actual user to logs
        response = donationHistoryService.addToHistory(locationId, fulfilledRequests, user);
        return ResponseEntity.status(response.getStatus()).body(response);
    }
}
