#include "gtest/gtest.h"

#include "Analytics/Model_BlackScholes.h"

using namespace tasty;


TEST(ModelBlackScholes, American)
{
    Model_BlackScholes model;
    {
        f64 vCall;
        ASSERT_EQ(model.priceAmerican(100., 100., kDaysInYear, 0.6, 0.05, 0.0, Parity_Call, vCall), "");
        ASSERT_NEAR(vCall, 25.5232, 0.0002);

        f64 vPut;
        ASSERT_EQ(model.priceAmerican(100., 100., kDaysInYear, 0.6, 0.05, 0.0, Parity_Put, vPut), "");
        ASSERT_NEAR(vPut, 21.1951, 0.0002);

        f64 zCall;
        ASSERT_EQ(model.calibrateAmerican(25.5232, 100., 100., kDaysInYear, 0.05, 0.0, Parity_Call, zCall), "");
        ASSERT_NEAR(zCall, 0.6, 0.0002);

        f64 zPut;
        ASSERT_EQ(model.calibrateAmerican(21.1951, 100., 100., kDaysInYear, 0.05, 0.0, Parity_Put, zPut), "");
        ASSERT_NEAR(zPut, 0.6, 0.0002);
    }

    {
        f64 vCall;
        ASSERT_EQ(model.priceAmerican(120., 100., kDaysInYear, -0.3, 0.05, 0., Parity_Call, vCall), "");
        ASSERT_NEAR(vCall, 20.8737, 0.0002);
        ASSERT_EQ(model.priceAmerican(120., 100., kDaysInYear,  0.3, 0.05, 0., Parity_Call, vCall), "");
        ASSERT_NEAR(vCall, 28.8804, 0.0002);

        f64 vPut;
        ASSERT_EQ(model.priceAmerican(100., 120., kDaysInYear, -0.3, 0.05, 0., Parity_Put, vPut), "");
        ASSERT_NEAR(vPut, 17.3186, 0.001);
        ASSERT_EQ(model.priceAmerican(100., 120., kDaysInYear,  0.3, 0.05, 0., Parity_Put, vPut), "");
        ASSERT_NEAR(vPut, 22.6814, 0.001);

        f64 zCall;
        ASSERT_EQ(model.calibrateAmerican(20., 120., 100., kDaysInYear, 0.05, 0.0, Parity_Call, zCall), "");
        ASSERT_NEAR(zCall, -0.3273, 0.0002);

        f64 zPut;
        ASSERT_EQ(model.calibrateAmerican(10., 100., 120., kDaysInYear, 0.05, 0.0, Parity_Put, zPut), "");
        ASSERT_NEAR(zPut, -0.4987, 0.0002);
    }
}


TEST(ModelBlackScholes, European)
{
    Model_BlackScholes model;

    f64 ivCall;
    ASSERT_EQ(model.calibrateEuropean(25.52, 100., 100., kDaysInYear, 0.05, 0.0, Parity_Call, ivCall), "");
    ASSERT_NEAR(ivCall, 0.6, 0.0002);

    f64 ivPut;
    ASSERT_EQ(model.calibrateEuropean(20.65, 100., 100., kDaysInYear, 0.05, 0.0, Parity_Put, ivCall), "");
    ASSERT_NEAR(ivCall, 0.6, 0.0002);
}
