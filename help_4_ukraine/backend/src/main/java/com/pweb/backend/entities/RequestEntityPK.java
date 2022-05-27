package com.pweb.backend.entities;

import javax.persistence.Column;
import javax.persistence.Id;
import java.io.Serializable;

public class RequestEntityPK implements Serializable {
    @Column(name = "location_id", nullable = false)
    @Id
    private int locationId;
    @Column(name = "item_name", nullable = false, length = 255)
    @Id
    private String itemName;

    public int getLocationId() {
        return locationId;
    }

    public void setLocationId(int locationId) {
        this.locationId = locationId;
    }

    public String getItemName() {
        return itemName;
    }

    public void setItemName(String itemName) {
        this.itemName = itemName;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        RequestEntityPK that = (RequestEntityPK) o;

        if (locationId != that.locationId) return false;
        if (itemName != null ? !itemName.equals(that.itemName) : that.itemName != null) return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = locationId;
        result = 31 * result + (itemName != null ? itemName.hashCode() : 0);
        return result;
    }
}
