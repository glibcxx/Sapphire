#pragma once

class BCMEditor; // Forward declaration

class KeyframeTableWidget {
public:
    explicit KeyframeTableWidget(BCMEditor &editor);
    void render();

private:
    BCMEditor &mEditor;
};