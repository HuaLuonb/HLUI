#pragma once
#include <string>
#include <functional>
#include <vector>
#include <memory>

struct Color {
    float r, g, b, a;
};

class Widget {
public:
    virtual ~Widget() = default;

    virtual void Draw() const = 0;
    virtual void HandleInput(float mouseX, float mouseY, bool mousePressed) = 0;

    void SetPosition(float x, float y) { posX = x; posY = y; }
    void SetSize(float width, float height) { this->width = width; this->height = height; }
    void SetColor(const Color& color) { this->color = color; }
    
    float GetPosX() const { return posX; }
    float GetPosY() const { return posY; }
    float GetWidth() const { return width; }
    float GetHeight() const { return height; }
    Color GetColor() const { return color; }

protected:
    float posX = 0, posY = 0;
    float width = 100, height = 30;
    Color color = {1.0f, 1.0f, 1.0f, 1.0f};

    bool IsMouseOver(float mouseX, float mouseY) const {
        return mouseX >= posX && mouseX <= posX + width &&
               mouseY >= posY && mouseY <= posY + height;
    }
};

class Checkbox : public Widget {
public:
    Checkbox(const std::string& label, bool* value)
        : label(label), value(value) {}

    void Draw() const override {}

    void HandleInput(float mouseX, float mouseY, bool mousePressed) override {
        if (mousePressed && IsMouseOver(mouseX, mouseY)) {
            *value = !*value;
        }
    }

private:
    std::string label;
    bool* value;
};

class Button : public Widget {
public:
    Button(const std::string& label, std::function<void()> action)
        : label(label), action(action) {}

    void Draw() const override {}

    void HandleInput(float mouseX, float mouseY, bool mousePressed) override {
        if (mousePressed && IsMouseOver(mouseX, mouseY)) {
            if (action) action();
        }
    }

private:
    std::string label;
    std::function<void()> action;
};

class Tab : public Widget {
public:
    enum class Alignment {
        Top,
        Left
    };

    Tab(const std::string& label, Alignment alignment)
        : label(label), alignment(alignment) {}

    void AddTab(std::unique_ptr<Widget> tabWidget) {
        tabs.push_back(std::move(tabWidget));
    }

    void SetAlignment(Alignment align) { alignment = align; }

    void Draw() const override {
        for (const auto& tab : tabs) {
            tab->Draw();
        }
    }

    void HandleInput(float mouseX, float mouseY, bool mousePressed) override {
        for (const auto& tab : tabs) {
            tab->HandleInput(mouseX, mouseY, mousePressed);
        }
    }

private:
    std::string label;
    Alignment alignment;
    std::vector<std::unique_ptr<Widget>> tabs;
};

class Window {
public:
    Window(const std::string& title) : title(title) {}

    void AddWidget(std::unique_ptr<Widget> widget) {
        widgets.push_back(std::move(widget));
    }

    void SetTitleColor(const Color& color) { titleColor = color; }
    void SetBackgroundColor(const Color& color) { backgroundColor = color; }

    void Draw() const {
        for (const auto& widget : widgets) {
            widget->Draw();
        }
    }

    void HandleInput(float mouseX, float mouseY, bool mousePressed) {
        for (const auto& widget : widgets) {
            widget->HandleInput(mouseX, mouseY, mousePressed);
        }
    }

private:
    std::string title;
    Color titleColor = {0.0f, 0.0f, 0.0f, 1.0f};
    Color backgroundColor = {1.0f, 1.0f, 1.0f, 1.0f};
    std::vector<std::unique_ptr<Widget>> widgets;
};
