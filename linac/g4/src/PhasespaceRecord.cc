//////////////////////////////////////////////////////////////////////////
// License & Copyright
// ===================
// 
// Copyright 2012 Christopher M Poole <mail@christopherpoole.net>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//////////////////////////////////////////////////////////////////////////


#include "globals.hh"

#include "PhasespaceRecord.hh"

#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"


PhasespaceRecord::PhasespaceRecord() {

}

PhasespaceRecord::PhasespaceRecord(G4Step* step) {
    const G4Track* track = step->GetTrack();

    position_x = track->GetPosition().x();
    position_y = track->GetPosition().y();
    position_z = track->GetPosition().z();

    momentum_x = track->GetMomentum().x();
    momentum_y = track->GetMomentum().y();
    momentum_z = track->GetMomentum().z();

    kinetic_energy = track->GetKineticEnergy();

    weight = track->GetWeight();    
    
    G4String type = track->GetParticleDefinition()->GetParticleName();
    if (type == "e+") {
        particle_type = 1;
    } else if (type == "e-") {
        particle_type = -1;
    } else {
        particle_type = 0;
    }
}

PhasespaceRecord::~PhasespaceRecord() {
}
/*
template<class Archive>
void PhasespaceRecord::serialize(Archive & ar, const unsigned int version);
    ar & position_x;
    ar & position_y;
    ar & position_z;

    ar & momentum_x;
    ar & momentum_y;
    ar & momentum_z;

    ar & kinetic_energy;
}
*/
G4bool PhasespaceRecord::CheckIt(G4double xlow, G4double xhigh,
        G4double ylow, G4double yhigh,
        G4double zlow, G4double zhigh)
{
    if ((isnan(position_x)) ||
        (isnan(position_y)) ||
        (isnan(position_z))) { 
        return false;
    }
    
    G4double tol_low = 1e-100;
    if (((std::abs(position_x) < tol_low) && (std::abs(position_x) > 0)) ||
        ((std::abs(position_y) < tol_low) && (std::abs(position_y) > 0)) ||
        ((std::abs(position_z) < tol_low) && (std::abs(position_z) > 0))) {
        return false;
    }

    G4double tol_high = 1e10; 
    if ((std::abs(position_x) > tol_high) ||
        (std::abs(position_y) > tol_high) ||
        (std::abs(position_z) > tol_high)) {
        return false;
    }

    if ((position_x < xlow) || (position_x > xhigh) ||
        (position_y < ylow) || (position_y > yhigh) ||
        (position_z < zlow) || (position_z > zhigh)) {
        return false;
    }

    return true;
}

G4ThreeVector PhasespaceRecord::GetPosition() {
    return G4ThreeVector(position_x, position_y, position_z);
}

G4ThreeVector PhasespaceRecord::GetMomentum() {
    return G4ThreeVector(momentum_x, momentum_y, momentum_z);
}

G4double PhasespaceRecord::GetKineticEnergy() {
    return kinetic_energy;
}

G4int PhasespaceRecord::GetParticleType() {
    return particle_type;
}

G4double PhasespaceRecord::GetWeight() {
    return weight;
}
