package com.pweb.backend.repositories;

import com.pweb.backend.entities.DonationHistoryEntity;
import com.pweb.backend.entities.DonationHistoryEntityPK;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface DonationHistoryRepository extends JpaRepository<DonationHistoryEntity, DonationHistoryEntityPK> {
    List<DonationHistoryEntity> findAllByLocationIdOrderByDonatedByDesc(int locationId);
}
