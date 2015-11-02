#apt-get install curl -y
#curl https://raw.githubusercontent.com/PETER-UWP/PETER/master/ros/x200/setup.sh|sh

## setup debian settings
sed -e 's/^GRUB_TIMEOUT[=].*$/GRUB_TIMEOUT=0/' /etc/default/grub > /etc/default/grub1
mv /etc/default/grub1 /etc/default/grub
update-grub&

## wifi drivers for the x200
# commented out becoues UWP has non-standard wifi protocals
# apt-get update && apt-get install firmware-iwlwifi
# modprobe -r iwlwifi ; modprobe iwlwifi

## setup ros
sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
apt-key adv --keyserver hkp://pool.sks-keyservers.net --recv-key 0xB01FA116
apt-get update
apt-get install aptitude git screen -y
apt-get install xserver-xorg-dev-lts-utopic mesa-common-dev-lts-utopic libxatracker-dev-lts-utopic libopenvg1-mesa-dev-lts-utopic libgles2-mesa-dev-lts-utopic libgles1-mesa-dev-lts-utopic libgl1-mesa-dev-lts-utopic libgbm-dev-lts-utopic libegl1-mesa-dev-lts-utopic ros-indigo-desktop-full -y
rosdep init
adduser ros
su ros -c 'rosdep update'
su ros -c 'echo "source /opt/ros/indigo/setup.bash" >> ~/.bashrc'
apt-get install python-rosinstall -y

## setup ros's atachment to git
su ros -c 'echo "sh ~/rosinit.sh">>~/.bashrc'
su ros -c 'mkdir ~/git'
su ros -c '(cd ~/git/ && git clone http://github.com/PETER-UWP/PETER.git)'
su ros -c 'sh ~/git/PETER/ros/x200/startup.sh'
