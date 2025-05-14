// Indicator.h
#pragma once

class Indicator {
public:
    // Increase price by exactly 2%
    static double increaseByTwoPercent(double price) {
        return price * 1.02;
    }

    // Increase price by any given percentage
    static double increaseByPercent(double price, double percent) {
        return price * (1.0 + percent / 100.0);
    }

    // Calculate Take Profit price
    static double takeProfit(double entryPrice, double percent) {
        return entryPrice * (1.0 + percent / 100.0);
    }

    // Calculate Stop Loss price
    static double stopLoss(double entryPrice, double percent) {
        return entryPrice * (1.0 - percent / 100.0);
    }
};
