package com.pweb.backend.repositories;

import com.pweb.backend.entities.LocationEntity;
import org.springframework.data.jpa.repository.JpaRepository;

public interface LocationsRepository extends JpaRepository<LocationEntity, Integer> {
    LocationEntity findLocationDetailsEntityById(int id);
}
