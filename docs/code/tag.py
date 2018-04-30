import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as spsn

def butter_lowpass(highcut, fs, order=5):
    nyq =  0.5 * fs
    high = highcut / nyq
    b, a = spsn.butter(order, high, btype='low')
    return b, a

def butter_highpass(lowcut, fs, order=5):
    nyq =  0.5 * fs
    low = lowcut / nyq
    b, a = spsn.butter(order, low, btype='high')
    return b, a

def butter_bandpass_filter(data, lowcut, highcut, fs, lporder=1, hporder=1):
    lpb, lpa = butter_lowpass(highcut, fs, order=lporder)
    hpb, hpa = butter_highpass(lowcut, fs, order=hporder)
    y = spsn.lfilter(lpb, lpa, data)
    y = spsn.lfilter(hpb, hpa, y)
    return y

stim_on = 0.5
stim_off = 2.5
time = np.arange(0., 3.5, 0.001)
stimulus = np.zeros(time.shape)
stimulus[(time >= stim_on) & (time < stim_off)] = 1;
response = butter_bandpass_filter(stimulus, .25, 10., 1./0.001)

fig = plt.figure()
fig.set_size_inches(5.5, 2.5)
ax = fig.add_subplot(111)
ax.plot(time, response, label="response")
ax.set_xlabel("time [s]")
ax.set_ylabel("voltage [mV]")
ax.plot([stim_on, stim_off], [1.0, 1.0], ls="solid", lw=2., color="red", label="stimulus on")
ax.set_ylim([-1.2, 1.2])
ax.set_yticks([-1., -0.5, 0, 0.5, 1.])
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.legend(loc=3, frameon=False)
fig.subplots_adjust(left=0.15, bottom=0.175, right=0.95, top=0.95)
fig.savefig('../images/tag1.png')

ax.scatter(stim_on, 1.15, marker="o", color='silver', lw=0.1)
ax.plot([stim_on, stim_off], [1.15, 1.15], lw=0.5, color="silver")
ax.plot([stim_off, stim_off], [1.1, 1.2], lw=0.5, color="silver")
ax.text(1.3, 1.175, "extent")
ax.annotate('position', xy=(stim_on, 1.15), xytext=(-0.15, 1.15),
            arrowprops=dict(facecolor='silver', connectionstyle="arc3", arrowstyle="->" ),
            )
fig.savefig('../images/tag2.png')
