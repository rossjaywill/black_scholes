# European Options - BSM
Black Scholes Merton -
Options Pricing

### Formulae

$$C = S_t . N(d_1) - Ke^-rt . N(d_2)$$

$$P = Ke^-rt . N(-d_2) - S_t . N(-d_1)$$


Given:

$$d_1 = \frac{ln . \frac{s_t}{K} + (r + \frac{σ^2_v}{2}t)}{σ_s . \sqrt{t}}$$

and

$$d_2 = d_1 - σ_s . \sqrt{t}$$

Where:

| Term | Definition |
| ----------- | ----------- |
| C | Call Option Price |
| P | Put Option Price |
| S | Underlying Asset |
| K | Strike |
| r | Interest Rate (Risk Free) |
| t | Time to expiry |
| N | Normal Distribution |
| σ | Std. Dv. Log Returns (Implied Volatility) |

