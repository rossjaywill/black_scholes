# European Options - BSM
Black Scholes Merton -
Options Pricing

### Formula

$$C = S_t . N*(d_1) - Ke^-rt . N(d_2)$$


Given:

$$d_1 = \frac{ln . \frac{s_t}{K} + (r + \frac{σ^2_v}{2}t)}{σ_s . \sqrt{t}}$$

and

$$d_2 = d_1 - σ_s . \sqrt{t}$$

and where:

| Term | Definition |
| ----------- | ----------- |
| C | Option Price |
| S | Underlying Asset |
| K | Strike |
| r | Interest Rate (Risk Free) |
| t | Time to expiry |
| N | Normal Distribution |
| σ | Std. Dv. Log Returns (Implied Volatility) |

