#ifndef MANAGEDDESCRIPTOR_H
#define MANAGEDDESCRIPTOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

class ObservationCounter {
protected:
    int observationCounter = 0;
};

class Event : virtual public ObservationCounter {
public:
    Event() = default;

    Event(const Event&) = delete;

    Event& operator=(const Event&) = delete;

    virtual ~Event() = default;

    void observe() {
        observationCounter++;
        enabled = true;
    }

    void unobserve() {
        observationCounter--;
        enabled = false;
    }

    void checkObserved() {
        if (observationCounter == 0) {
            unobserved();
        }
    }

    bool isEnabled() const {
        return enabled;
    }

protected:
    virtual void enable() = 0;
    virtual void disable() = 0;

    virtual void unobserved() = 0;

private:
    bool enabled = false;
};

#endif // MANAGEDDESCRIPTOR_H
