package com.pweb.backend;

import com.pweb.backend.services.DonationHistoryService;
import com.pweb.backend.services.LocationsService;
import com.pweb.backend.services.MQService;
import com.pweb.backend.services.RequestsService;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import java.util.Objects;

@SpringBootTest
class BackendApplicationTests {
	private final LocationsService locationsService;
	private final RequestsService requestsService;
	private final DonationHistoryService donationHistoryService;
	private final MQService mqService;

	@Autowired
	public BackendApplicationTests(LocationsService locationsService,
								   RequestsService requestsService,
								   DonationHistoryService donationHistoryService,
								   MQService mqService) {
		this.locationsService = locationsService;
		this.requestsService = requestsService;
		this.donationHistoryService = donationHistoryService;
		this.mqService = mqService;
	}

	@Test
	void contextLoads() {
	}

	@Test
	void getAllLocations() {
		assert(Objects.nonNull(locationsService.getAllLocationDetails()));
	}

	@Test
	void getAllRequests() {
		assert(requestsService.getRequestsByLocation(-1, "test", "admin").getRequests().isEmpty());
	}

	@Test
	void getAllDonations() {
		assert(donationHistoryService.getHistoryForLocation(-1, "test").isEmpty());
	}

	@Test
	void checkBrokerConnection() {assert(mqService.isConnected());}
}
