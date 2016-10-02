# Restaurant Simulator C++

## Running

```sh
$ git clone https://github.com/juanmanuelromeraferrio/resto-simulation-c-.git # or clone your own fork
```

## Building using Eclipse

1. From the main menu bar, select  command link File > Import.... The Import wizard opens.
2. Select General > Existing Project into Workspace and click Next.
3. Choose either Select root directory or Select archive file and click the associated Browse to locate the directory or file containing the projects.
4. Under Projects select the project or projects which you would like to import.
5. Click Finish to start the import.
6. Build Project

## Running Simulation

To run de Host, Waiters and Cook process.

```sh
$ cd /tmp/
$ mkdir resto
$ cd resto
$ touch restaurant.tmp
$ cd /github_folder_path/resto-simulation-c-/Debug
$ ./SimulatorRestaurant -i
```

To run Diner's process.
numOfDiners is the number of diners to enter the Restaurant.

```sh
$ cd resto-simulation-c-/Debug
$ ./SimulatorRestaurant -d numOfDiners
```

