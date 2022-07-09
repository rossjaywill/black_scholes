# Black Scholes Merton
European Options Pricing

### Usage
```
Direct run of BSM command requires at least 4 of the required arguments:
        -o | --option-type      : Type of option ('call' or 'put')       [required]
        -u | --underling-price  : Price of underlying asset              [required]
        -s | --strike-price     : Strike price of the options contract   [required]
        -t | --time-to-expiry   : Time to expiry date of the option      [required]
                                  [next (front) expiry in YYYY-mm-dd format]
        -v | --volatility       : Implied volatility of underlying asset [optional]
        -r | --rate-of-interest : Risk Free interest rate                [optional]

        -d | --dividend-yield   : Dividend yield rate                    [optional]

        [-h | --help            : Display this help/usage message]
Optionally, if volatility, interest-rate, or dividend yield are omitted,
  the program will use default assumptions for these values, of:
  18% volatilty, 2% interest rate, 0% dividend yield.
```

Examples:

Running by passing csv as standard in:
```bash
cat tst/input/bsm.csv | ./build/bin/bsm
```

Call option with defaulted volatility and rates:
```bash
bsm -o call -u 150 -s 100 -t 2022-07-30
```

Put option with volatility and rates specified:
```bash
bsm -o put -u 35.75 -s 42.80 -t 2022-07-30 -r 0.03 -v 0.15 -d 0.05
```


### Building

Build information can here found [here](./docs/BUILD.md), including system dependency information.

### Formulae

$$C = S_te^{-r_ft} . N(d_1) - Ke^{-r_dt} . N(d_2)$$

$$P = Ke^{-r_dt} . N(-d_2) - S_te^{-r_ft} . N(-d_1)$$


Given:

$$d_1 = \frac{ln . \frac{s_t}{K} + (r_d - r_f + \frac{σ^2_v}{2}t)}{σ_s . \sqrt{t}}$$

and

$$d_2 = d_1 - σ_s . \sqrt{t}$$

[NB: Greeks' formulae can here found [here](./docs/GREEKS.md)]

Where:

| Term | Flag | Definition |
| --- | ---- |  ------------- |
| C | - | Call Option Price (Premium) |
| P | - | Put Option Price (Premium) |
| S(t) | -u | Underlying Asset's Price |
| K | -s | Strike Price |
| t | -t | Time to expiry |
| σ | -v | Std. Dv. Log Returns (Implied Volatility) |
| r(d) | -r | (Risk Free) Interest Rate |
| r(f) | -d | Dividend Yield rate |
| N | - | Cumulative Normal Distribution |
| n | - | Standard Normal Density |

### Future Work:

1. Implement input from stream of serialised data; csv, json, etc via standard in,
1a. Run benchmarks of the above
2. Extend dividend yield testing,
3. Extend memoization of common terms in black scholes calculations (in line with greeks).
4. Add windows build
5. Improve/Extend error scenario testing

Thanks to:

Columbia University E4706: Foundations of Financial Engineering © 2016 by Martin Haugh:
> http://www.columbia.edu/~mh2078/FoundationsFE/BlackScholes.pdf

and
quantpie.co.uk:
> https://quantpie.co.uk/bsm_formula/bs_summary.php

For equation derivations.

MyStockPlan.com, Inc Copyright © 2000-2022 myStockPlan.com:
> https://www.mystockoptions.com/black-scholes.cfm

For providing a tool to test against.
