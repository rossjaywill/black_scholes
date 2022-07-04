## Greeks
Equations used to derive greeks for European options.

### Call Options

Delta:
$$ e^{-r_ft} . N(d1)  $$

Gamma:
$$ \frac{e^{-r_ft}}{S_tσ_s\sqrt{t}}.n(d1) $$

Vega:
$$ S_te^{-r_ft}.\sqrt{t}.n(d1) $$

Rho:
$$ Ke^{-r_dt}t.N(d1 - σ_s\sqrt{t}) $$

Theta:
$$ S_te^{r_ft}r_f.N(d1) - Ke^{-r_dt}r_d.N(d1 - σ_s\sqrt{t}) -S_te^{-r_ft}\frac{σ_s}{2\sqrt{t}}.n(d1)  $$

### Put Options

Delta:
$$ -e^{-r_ft} . N(-d1)  $$

Gamma:
$$ \frac{e^{-r_ft}}{S_tσ_s\sqrt{t}}.n(d1) $$

Vega:
$$ S_te^{-r_ft}.\sqrt{t}.n(d1) $$

Rho:
$$ -Ke^{-r_dt}t.N(-d1 + σ_s\sqrt{t}) $$

Theta:
$$ -S_te^{r_ft}r_f.N(-d1) + Ke^{-r_dt}r_d.N(-d1 + σ_s\sqrt{t}) -S_te^{-r_ft}\frac{σ_s}{2\sqrt{t}}.n(d1)  $$
