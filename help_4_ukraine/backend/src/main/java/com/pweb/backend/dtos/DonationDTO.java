package com.pweb.backend.dtos;

import com.pweb.backend.entities.DonationHistoryEntity;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class DonationDTO {
    int locationId;
    String itemName;
    int count;
    String donatedBy;
    String timestamp;

    public DonationDTO(DonationHistoryEntity entity) {
        locationId = entity.getLocationId();
        itemName = entity.getItemName();
        count = entity.getCount();
        donatedBy = entity.getDonatedBy();
        timestamp = entity.getTime().toString();
    }
}
