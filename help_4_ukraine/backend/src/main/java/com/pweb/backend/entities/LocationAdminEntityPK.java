package com.pweb.backend.entities;

import javax.persistence.Column;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import java.io.Serializable;

public class LocationAdminEntityPK implements Serializable {
    @Column(name = "location_id", nullable = false)
    @Id
    private int locationId;
    @Column(name = "email", nullable = false, length = 255)
    @Id
    private String email;

    public int getLocationId() {
        return locationId;
    }

    public void setLocationId(int locationId) {
        this.locationId = locationId;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        LocationAdminEntityPK that = (LocationAdminEntityPK) o;

        if (locationId != that.locationId) return false;
        if (email != null ? !email.equals(that.email) : that.email != null) return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = locationId;
        result = 31 * result + (email != null ? email.hashCode() : 0);
        return result;
    }
}
