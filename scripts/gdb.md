# Commands
```bash
	# run on pc connected to rp2040
	sudo openocd -f ./openocd.cfg -s tcl

	# run on dev pc
	gdb-multiarch -ex 'target extended-remote 192.168.0.52:3333' SIDPod-SDCard.elf
```
