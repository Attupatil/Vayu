ISO_NAME = Vayu-x86_64.iso

all: scripts/build_vayu.sh
	./scripts/build_vayu.sh

clean:
	rm -rf rootfs live $(ISO_NAME)
