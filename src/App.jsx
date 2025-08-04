import { useState } from 'react';
import { AnimatePresence, motion } from 'framer-motion';
import SplashScreen from './screens/SplashScreen';
import TransitionScreen from './screens/TransitionScreen';
import CodeScreen from './screens/CodeScreen';
import './styles/globals.css';

function App() {
  const [step, setStep] = useState(0);

  return (
    <div style={{
      display: "flex",
      justifyContent: "center",
      alignItems: "center",
      height: "100vh",
      width: "100vw"
    }}>
      <AnimatePresence mode="wait">
        {step === 0 && (
          <motion.div
            key="splash"
            style={{ width: "100%" }}
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            exit={{ opacity: 0 }}
            transition={{ duration: 0.8 }}
          >
            <SplashScreen onComplete={() => setStep(1)} />
          </motion.div>
        )}
        {step === 1 && (
          <motion.div
            key="transition"
            style={{ width: "100%" }}
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            exit={{ opacity: 0 }}
            transition={{ duration: 0.6 }}
          >
            <TransitionScreen onComplete={() => setStep(2)} />
          </motion.div>
        )}
        {step === 2 && (
          <motion.div
            key="code"
            style={{ width: "100%" }}
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            exit={{ opacity: 0 }}
            transition={{ duration: 0.8 }}
          >
            <CodeScreen />
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  );
}

export default App;