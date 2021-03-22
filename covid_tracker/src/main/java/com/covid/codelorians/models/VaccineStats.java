package com.covid.codelorians.models;

import org.json.JSONObject;

// Class for storing data about vaccine candidates
public class VaccineStats implements Comparable<VaccineStats> {
    private String candidate, mechanism, sponsor, phase, institution, details;
    private int id;

    public VaccineStats(JSONObject object, int id) {
        candidate = object.getString("candidate");
        mechanism = object.getString("mechanism");
        sponsor = object.getJSONArray("sponsors").join(",");
        phase = object.getString("trialPhase");
        institution = object.getJSONArray("institutions").join(",");
        details = object.getString("details");
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public String getCandidate() {
        return candidate;
    }

    public String getMechanism() {
        return mechanism;
    }

    public String getSponsor() {
        return sponsor;
    }

    public String getPhase() {
        return phase;
    }

    public String getInstitution() {
        return institution;
    }

    public String getDetails() {
        return details;
    }

    // Obtains numeric value for Trial Phase
    public float getPhaseNr() {
        if (phase.equals("Phase 3")) {
            return 3;
        }
        if (phase.equals("Phase 2/3")) {
            return 2.5f;
        }
        if (phase.equals("Phase 2")) {
            return 2;
        }
        if (phase.equals("Phase 1/2")) {
            return 1.5f;
        }
        if (phase.equals("Phase 1")) {
            return 1;
        }
        return 0;
    }

    @Override
    public int compareTo(VaccineStats o) {
        return Float.compare(getPhaseNr(), o.getPhaseNr());
    }
}
