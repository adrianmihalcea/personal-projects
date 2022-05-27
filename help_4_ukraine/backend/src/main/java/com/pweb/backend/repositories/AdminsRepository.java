package com.pweb.backend.repositories;

import com.pweb.backend.entities.LocationAdminEntity;
import com.pweb.backend.entities.LocationAdminEntityPK;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface AdminsRepository extends JpaRepository<LocationAdminEntity, LocationAdminEntityPK> {

    List<LocationAdminEntity> findLocationAdminEntitiesByLocationId(int locationId);

    boolean existsByEmailAndLocationId(String email, int locationId);
}
