Multipass
Multipass is a multi-platform tool for allowing users to easily spin up Ubuntu Cloud instances. It’s written in C++ and interfaces with a number of 3rd party libraries and technologies. Multipass strives to have a clean and concise CLI without adding too many options to burden end users.

Prerequisites
CMake
libcurl
RapidJSON
Building

Clone the repository:


$ git clone https://github.com/florencelourdes/multipass.git
$ cd multipass
Build the project using CMake:


$ cmake .
$ make
Running
To run Multipass, execute the following command:


$ ./multipass
Usage
Once Multipass is running, you can use the following commands:

list-releases: Lists all currently supported Ubuntu releases.
current-lts: Returns the current Ubuntu LTS version.
sha256 <release>: Returns the sha256 of the disk1.img item of a given Ubuntu release.
Example usage:


$ ./multipass list-releases
$ ./multipass current-lts
$ ./multipass sha256 bionic
Contributing
We welcome contributions to Multipass. If you would like to contribute, please fork the repository and submit a pull request following our contribution guidelines.

License
Multipass is licensed under the MIT License.

