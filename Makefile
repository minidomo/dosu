# https://stackoverflow.com/a/36683917
define GetFromConfig
$(shell node -p "require('./config.json').$(1)")
endef

GODOT_CPP_PATH := $(call GetFromConfig,godotcpp.path)
IS_LAB := $(call GetFromConfig,isLab)

ifeq ($(IS_LAB), true)
	SCONS = python3 /usr/bin/scons
else
	SCONS = scons
endif

BINDINGS_FLAGS = -C "${GODOT_CPP_PATH}" -j4 bits=64

bindings:
	${SCONS} ${BINDINGS_FLAGS}
	
plugin:
	${SCONS} --no-cache

plugin-clean:
	${SCONS} --clean