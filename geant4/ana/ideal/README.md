# Summary of the workflow

The file `run.sh` contains a set of commands that summarise the workflow.
The workflow is the following

1. clean files
2. start simulation
3. merge the output files
4. analyse the merged output

The workflow is repeated for the two B field cases.

# Running the simulation

There are two main Geant4 macros

```
ideal.Bzero.mac
ideal.Bmap.mac
```

that are basically identical.
The only difference is in the settings of the magnetic field and in the output file prefix.

Both call another G4 macro with the command

```
/control/execute ideal.run.mac
```

which is responsible for starting the run, in a way that the same is used for both magnetic field settings.

There are several aliases, that are used for the following loops.
The first loop is over `Particles` and it is controlled by

```
/control/foreach particle.loop Particle {Particles}
```

which will execute the `particle.loop` G4 macro for each element in `Particles`.
Each element in `Particles` will be aliased by `Particle`.

The code now enters in the `particle.loop` G4 macro, where the particle is set.
Then there is a loop over `Directions` following the same strategy and executing `direction.loop`.
Then there is a loop over `Momenta` following the same strategy and executing `momentum.loop`.

In `momentum.loop` we really start the run with `/run/beamOn` and we use the `nEvents` alias defined in `ideal.run.mac` to fires a given number of particles.

So, this does the job of starting multiple Geant4 runs with different conditions.

# Analysis of the output

For the analysis, there is the `ideal_ring.C` macro that takes an input file and gives as output another file.
In the output file there is a TGraph2D object with the value of the RMS of the single-photon angular distribution as a function of eta and p.