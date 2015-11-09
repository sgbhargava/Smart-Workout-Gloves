#ifndef I2C1_HPP_
#define I2C1_HPP_

#include "singleton_template.hpp"
#include "i2c_base.hpp"

class I2C1 : public I2C_Base, public SingletonTemplate<I2C1>
{
    public:
        /// Initializes I2C2 at the given @param speedInKhz
        bool init(unsigned int speedInKhz);

    private:
        I2C1(); ///< Private constructor for this singleton class
        friend class SingletonTemplate<I2C1>;  ///< Friend class used for Singleton Template
};


#endif
