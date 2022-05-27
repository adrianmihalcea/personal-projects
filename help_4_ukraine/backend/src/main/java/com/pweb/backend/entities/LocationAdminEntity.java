package com.pweb.backend.entities;

import lombok.AllArgsConstructor;
import lombok.NoArgsConstructor;

import javax.persistence.*;

@Entity
@Table(name = "LOCATION_ADMINS", schema = "db", catalog = "db")
@IdClass(LocationAdminEntityPK.class)
@AllArgsConstructor
@NoArgsConstructor
public class LocationAdminEntity {
    @Id
    @Column(name = "location_id", nullable = false)
    private int locationId;
    @Id
    @Column(name = "email", nullable = false, length = 255)
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

        LocationAdminEntity that = (LocationAdminEntity) o;

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
