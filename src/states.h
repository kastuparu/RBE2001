#pragma once 

const enum RoofType {
    DEG_45,
    DEG_25
} ROOF_TYPE = DEG_45;

const enum FieldSide {
    LEFT,                   // when traveling from the platform to the roof, turn left at the intersection
    RIGHT                   // when traveling from the platform to the roof, turn right at the intersection
} FIELD_SIDE = LEFT;

const enum GripperType {
    BOTTOM_OUT,
    LINEAR
} GRIPPER_TYPE = BOTTOM_OUT;

enum StateRobot1 {
    PREP_ROOF_REMOVAL,      // move fourbar to pre-roof position, move to appropriate distance, open gripper
    GRIP_ROOF_PANEL,        // move fourbar to roof position, move appropriate distance, close gripper, wait for signal
    REMOVE_ROOF_PANEL,      // move fourbar to pre-roof position, back up
    REMOVE_ROOF_PANEL_TURN, // turn 180 deg
    DRIVE_ROOF_TO_CROSS,    // move fourbar to pre-platform position, line follow to intersection
    DRIVE_ROOF_TO_CROSS_2,  // move forward over intersection
    DRIVE_ROOF_TO_CROSS_3,  // turn 90 deg
    DRIVE_CROSS_TO_PLATFORM,// line follow to platform
    PLACE_PLATFORM_PANEL,   // move fourbar to platform position, wait for signal
    RELEASE_PLATFORM_PANEL, // open gripper, wait for signal
    GRIP_PLATFORM_PANEL,    // close gripper, move fourbar to pre-platform position, move to pre-platform position
    REMOVE_PLATFORM_PANEL,  // turn 180 deg
    DRIVE_PLATFORM_TO_CROSS,// line follow to intersection
    DRIVE_PLATFORM_TO_CROSS_2, // move forward over intersection
    DRIVE_PLATFORM_TO_CROSS_3, // turn 90 deg
    DRIVE_CROSS_TO_ROOF,    // move fourbar to pre-roof position, line follow to roof
    PLACE_ROOF_PANEL,       // move fourbar to roof position, move appropriate distance, wait for signal
    RELEASE_ROOF_PANEL,     // open gripper, move fourbar to pre-roof position
    RELEASE_ROOF_PANEL_TURN, // turn 180 deg
    DRIVE_2_ROOF_TO_CROSS,    // move fourbar to pre-platform position, line follow to intersection
    DRIVE_2_ROOF_TO_CROSS_2,  // move forward over intersection
    DRIVE_2_ROOF_TO_CROSS_3,  // turn 90 deg
    DRIVE_2_CROSS_TO_PLATFORM,// line follow to platform
    DRIVE_2_CROSS_TO_PLATFORM_TURN, // turn 90 deg
    DRIVE_ACROSS_FIELD      // navigate across field until line, wait for signal
};