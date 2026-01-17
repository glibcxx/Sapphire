#pragma once

namespace ui {

    enum class LayoutAxisType : int {
        Absolute = 0,
        ParentRelativeX = 1,
        ParentRelativeY = 2,
        SelfRelativeX = 3,
        SelfRelativeY = 4,
        SizeOfChildrenX = 5,
        SizeOfChildrenY = 6,
        SizeOfLargestChildX = 7,
        SizeOfLargestChildY = 8,
        SizeOfMaxSiblingWidth = 9,
        SizeOfMaxSiblingHeight = 10,
        None = 11,
        Count = 12,
    };

    enum class LayoutAxisOperation : int {
        Add = 0,
        Subtract = 1,
        None = 2,
        Count = 3,
    };

    enum class LayoutAxisOffsetContainerType : int {
        Default = 0,
        Specified = 1,
        Fill = 2,
        None = 3,
        Count = 4,
    };

    // size: 16
    struct SliceSize {
    public:
        float left;   // off+0
        float top;    // off+4
        float right;  // off+8
        float bottom; // off+12
    };

    // size: 12
    struct AxisOffset {
        LayoutAxisType      type;      // off+0
        float               value;     // off+4
        LayoutAxisOperation operation; // off+8
    };

    // size: 56
    struct LayoutOffset {
        std::vector<AxisOffset>       x;     // off+0
        std::vector<AxisOffset>       y;     // off+24
        LayoutAxisOffsetContainerType xType; // off+48
        LayoutAxisOffsetContainerType yType; // off+52
    };

    enum class ViewRequest : uint32_t {
        None = 0,
        Refresh = 1,
        PointerHeldEventsRequest = 2,
        PointerHeldEventsCancel = 4,
        DelayedFocusRefresh = 8,
        Exit = 16,
        ConsumeEvent = 32,
    };

    enum class Draggable : char {
        NotDraggable = 0,
        Horizontal = 1,
        Vertical = 2,
        Both = 3,
    };

    enum class CardinalDirection : char {
        None = 0,
        Up = 1,
        Down = 2,
        Left = 3,
        Right = 4,
    };

    enum class NameResolutionScope : int {
        Global = 0,
        Local = 1,
        Sibling = 2,
    };

    enum class TileDirection : int {
        None = 0,
        X = 1,
        Y = 2,
        Both = 3,
    };

    enum class VisibilityFlag : uint32_t {
        None = 0,
        Visible = 1,
        VisibleInStackPanel = 2,
        All = 3,
    };

} // namespace ui