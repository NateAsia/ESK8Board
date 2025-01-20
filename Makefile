# Use bash as the shell
SHELL := /bin/bash

# Directories
BOARD_DIR := BoardControllerFirmware
HAND_DIR := HandTransmitterFirmware
COMMS_DIR := ESK8Comms

# ESK8Comms files
COMMS_FILES := ESK8Comms.h ESK8Comms.cpp

# Default target
.PHONY: default
default: copy_comms

# Target to copy files to both projects
.PHONY: copy_comms
copy_comms: copy_to_board copy_to_hand

# Copy to board controller
.PHONY: copy_to_board
copy_to_board:
	@echo "Copying ESK8Comms files to $(BOARD_DIR)..."
	@for file in $(COMMS_FILES); do \
		cp $(COMMS_DIR)/$$file $(BOARD_DIR)/; \
	done

# Copy to hand transmitter
.PHONY: copy_to_hand
copy_to_hand:
	@echo "Copying ESK8Comms files to $(HAND_DIR)..."
	@for file in $(COMMS_FILES); do \
		cp $(COMMS_DIR)/$$file $(HAND_DIR)/; \
	done

# Clean copied files
.PHONY: clean_comms
clean_comms:
	@echo "Removing ESK8Comms files from projects..."
	@for file in $(COMMS_FILES); do \
		rm -f $(BOARD_DIR)/$$file $(HAND_DIR)/$$file; \
	done