# Bmap

## Build the software

1. Clone the repository with `git clone https://github.com/preghenella/Bmap.git` or with `git clone git@github.com:preghenella/Bmap.git`. A `Bmap` directory will be created with the `git` source tree in it.
2. Create a `build` directory which is from where you will biuld the code, possibly `mkdir Bmap/geant4/build` and go there `cd Bmap/geant4/build`.
3. From within the `build` directory run `cmake ../.` and then `make install`.
4. The software should compile and the executable `Bmap` will be installed in `Bmap/geant4/bin`.

## Run the software

1. Create a `run` directory from where you will run the simulation, possibly `mkdir Bmap/geant4/run`.
2. Copy the relevant files in there
   * the executable `Bmap/geant4/bin/Bmap`
   * the field map file `Bmap/data/EIC_Magnetic_Field_Map_2021_04_30_cart_coords_[mm]_[T].Bmap`
   * the geant configuration file `Bmap/geant4/cfg/Bmap.mac`
3. Run the simulation from within the `run` directory with `./Bmap Bmap.mac`.
