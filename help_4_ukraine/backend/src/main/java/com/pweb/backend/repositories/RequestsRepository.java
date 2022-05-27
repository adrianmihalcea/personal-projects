package com.pweb.backend.repositories;

import com.pweb.backend.entities.RequestEntity;
import com.pweb.backend.entities.RequestEntityPK;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface RequestsRepository extends JpaRepository<RequestEntity, RequestEntityPK> {
    List<RequestEntity> findAllByLocationIdOrderByPriorityDesc(int locationId);
    RequestEntity findByLocationIdAndItemName(int locationId, String itemName);
    List<RequestEntity> findAllByLocationIdAndItemNameIsIn(int locationId, List<String> itemNames);
}
