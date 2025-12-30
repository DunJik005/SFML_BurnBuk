//
// Created by lazab on 12/20/2025.
//

#ifndef SFMLPROJECT_ONRESIZE_H
#define SFMLPROJECT_ONRESIZE_H


class OnResize {
public:
    virtual ~OnResize();
    virtual void onResize(float w, float h) = 0;
};

#endif