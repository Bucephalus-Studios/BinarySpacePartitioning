#include <gtest/gtest.h>
#include "test_helpers.hpp"
#include "../classes/BSP_Tree/classes/BSP_Node/classes/BSP_Rectangle.hpp"

/**
 * Test suite for BSP_Rectangle class
 */
class BSP_RectangleTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

// Test default constructor
TEST_F(BSP_RectangleTest, DefaultConstructor) {
    BSP_Rectangle rect;
    EXPECT_EQ(rect.x, 0);
    EXPECT_EQ(rect.y, 0);
    EXPECT_EQ(rect.width, 0);
    EXPECT_EQ(rect.height, 0);
}

// Test parameterized constructor
TEST_F(BSP_RectangleTest, ParameterizedConstructor) {
    BSP_Rectangle rect(10, 20, 100, 200);
    EXPECT_EQ(rect.x, 10);
    EXPECT_EQ(rect.y, 20);
    EXPECT_EQ(rect.width, 100);
    EXPECT_EQ(rect.height, 200);
}

// Test geometric center calculation
TEST_F(BSP_RectangleTest, GeometricCenter) {
    BSP_Rectangle rect(0, 0, 100, 200);
    auto center = rect.getCenter();
    EXPECT_EQ(std::get<0>(center), 50);  // x + width/2 = 0 + 100/2 = 50
    EXPECT_EQ(std::get<1>(center), 100); // y + height/2 = 0 + 200/2 = 100
}

// Test geometric center with offset
TEST_F(BSP_RectangleTest, GeometricCenterWithOffset) {
    BSP_Rectangle rect(50, 100, 200, 300);
    auto center = rect.getCenter();
    EXPECT_EQ(std::get<0>(center), 150); // 50 + 200/2 = 150
    EXPECT_EQ(std::get<1>(center), 250); // 100 + 300/2 = 250
}

// Test custom center constructor
TEST_F(BSP_RectangleTest, CustomCenterConstructor) {
    auto customCenter = std::make_tuple(75, 125);
    BSP_Rectangle rect(0, 0, 100, 200, customCenter);
    auto center = rect.getCenter();
    EXPECT_EQ(std::get<0>(center), 75);
    EXPECT_EQ(std::get<1>(center), 125);
}

// Test area calculation
TEST_F(BSP_RectangleTest, AreaCalculation) {
    BSP_Rectangle rect(0, 0, 10, 20);
    EXPECT_EQ(rect.getArea(), 200);
}

// Test area calculation with different dimensions
TEST_F(BSP_RectangleTest, AreaCalculationVariousSizes) {
    BSP_Rectangle rect1(0, 0, 1, 1);
    EXPECT_EQ(rect1.getArea(), 1);

    BSP_Rectangle rect2(0, 0, 100, 50);
    EXPECT_EQ(rect2.getArea(), 5000);

    BSP_Rectangle rect3(10, 10, 25, 40);
    EXPECT_EQ(rect3.getArea(), 1000);
}

// Test getWidth method
TEST_F(BSP_RectangleTest, GetWidth) {
    BSP_Rectangle rect(0, 0, 123, 456);
    EXPECT_EQ(rect.getWidth(), 123);
}

// Test getHeight method
TEST_F(BSP_RectangleTest, GetHeight) {
    BSP_Rectangle rect(0, 0, 123, 456);
    EXPECT_EQ(rect.getHeight(), 456);
}

// Test zero-sized rectangle
TEST_F(BSP_RectangleTest, ZeroSizedRectangle) {
    BSP_Rectangle rect(0, 0, 0, 0);
    EXPECT_EQ(rect.getArea(), 0);
    auto center = rect.getCenter();
    EXPECT_EQ(std::get<0>(center), 0);
    EXPECT_EQ(std::get<1>(center), 0);
}
