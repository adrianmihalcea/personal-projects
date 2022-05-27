package com.pweb.backend.entities;

import lombok.AllArgsConstructor;
import lombok.NoArgsConstructor;

import javax.persistence.*;
import java.sql.Timestamp;
import java.util.Objects;

@Entity
@Table(name = "DONATION_HISTORY", schema = "db", catalog = "db")
@IdClass(DonationHistoryEntityPK.class)
@AllArgsConstructor
@NoArgsConstructor
public class DonationHistoryEntity {
    @Id
    @Column(name = "location_id", nullable = false)
    private int locationId;
    @Id
    @Column(name = "item_name", nullable = false)
    private String itemName;
    @Basic
    @Column(name = "count", nullable = false)
    private int count;
    @Basic
    @Column(name = "donated_by", nullable = false)
    private String donatedBy;
    @Id
    @Column(name = "time", nullable = false)
    private Timestamp time;

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

    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }

    public String getDonatedBy() {
        return donatedBy;
    }

    public void setDonatedBy(String donatedBy) {
        this.donatedBy = donatedBy;
    }

    public Timestamp getTime() {
        return time;
    }

    public void setTime(Timestamp time) {
        this.time = time;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        DonationHistoryEntity that = (DonationHistoryEntity) o;

        if (locationId != that.locationId) return false;
        if (count != that.count) return false;
        if (!Objects.equals(itemName, that.itemName)) return false;
        if (!Objects.equals(donatedBy, that.donatedBy)) return false;
        if (!Objects.equals(time, that.time)) return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = locationId;
        result = 31 * result + (itemName != null ? itemName.hashCode() : 0);
        result = 31 * result + count;
        result = 31 * result + (donatedBy != null ? donatedBy.hashCode() : 0);
        result = 31 * result + (time != null ? time.hashCode() : 0);
        return result;
    }
}
