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

| Term | Flag | Definition |
| --- | ---- |  ------------- |
| C | - | Call Option Price (Premium) |
| P | - | Put Option Price (Premium) |
| S | -u | Underlying Asset |
| K | -s | Strike |
| t | -t | Time to expiry |
| σ | -v | Std. Dv. Log Returns (Implied Volatility) |
| r | -r | (Risk Free) Interest Rate |
| N | - | Cumulative Normal Distribution |
| n | - | Standard Normal Density |

