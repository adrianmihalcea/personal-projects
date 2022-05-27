package com.pweb.backend.entities;

import lombok.AllArgsConstructor;
import lombok.NoArgsConstructor;

import javax.persistence.*;
import java.util.Objects;

@Entity
@Table(name = "REQUESTS", schema = "db", catalog = "db")
@IdClass(RequestEntityPK.class)
@AllArgsConstructor
@NoArgsConstructor
public class RequestEntity {
    @Id
    @Column(name = "location_id", nullable = false)
    private int locationId;
    @Id
    @Column(name = "item_name", nullable = false, length = 255)
    private String itemName;
    @Basic
    @Column(name = "count", nullable = false)
    private int count;
    @Basic
    @Column(name = "priority", nullable = false, columnDefinition = "integer default 0")
    private Integer priority;

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

    public Integer getPriority() {
        return priority;
    }

    public void setPriority(Integer priority) {
        this.priority = priority;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        RequestEntity that = (RequestEntity) o;

        if (locationId != that.locationId) return false;
        if (count != that.count) return false;
        if (!Objects.equals(itemName, that.itemName)) return false;
        if (!Objects.equals(priority, that.priority)) return false;

        return true;
    }

    @Override
    public int hashCode() {
        int result = locationId;
        result = 31 * result + (itemName != null ? itemName.hashCode() : 0);
        result = 31 * result + count;
        result = 31 * result + (priority != null ? priority.hashCode() : 0);
        return result;
    }
}
