#! /usr/bin/env bash

rm -rf ideal.Bzero.*.root
./Bmap ideal.Bzero.mac
hadd -f ideal.Bzero.root ideal.Bzero.*.root && rm -rf ideal.Bzero.*.root
root -b -q -l "ideal_ring.C(\"ideal.Bzero.root\", \"ideal.Bzero.ring.root\")"

rm -rf ideal.Bmap.*.root
./Bmap ideal.Bmap.mac
hadd -f ideal.Bmap.root ideal.Bmap.*.root && rm -rf ideal.Bmap.*.root
root -b -q -l "ideal_ring.C(\"ideal.Bmap.root\", \"ideal.Bmap.ring.root\")"

