package xyz.snaker.natives;

import java.io.IOException;
import java.net.URL;
import java.nio.file.Path;
import java.nio.file.Paths;

import xyz.snaker.snkr4j.SimpleLogger;
import xyz.snaker.snkr4j.SnakerLogger;

/**
 * Created by SnakerBone on 4/11/2023
 **/
public class SnakerNatives
{
    public static final SnakerLogger LOGGER = new SimpleLogger(SnakerNatives.class, true);

    static {
        initialize();
    }

    private static void initialize()
    {
        ClassLoader loader = SnakerNatives.class.getClassLoader();
        URL resource = loader.getResource("snkr.dll");

        if (resource == null) {
            LOGGER.error("Could not load libraries for SnakerNatives: snkr.dll does not exist");
            return;
        }

        try {
            Path path = Paths.get(resource.toURI());

            if (!path.isAbsolute()) {
                path = path.toAbsolutePath();
            }

            if (!path.isAbsolute()) {
                path = Paths.get(path.toAbsolutePath().toString()).toAbsolutePath();
            }

            if (!path.isAbsolute()) {
                throw new IOException("Could not make path absolute: " + path);
            }

            System.load(path.toString());
        } catch (Exception e) {
            LOGGER.errorf("Could not load libraries for SnakerNatives: []", e.getMessage());
            return;
        }

        LOGGER.infof("Loaded native libraries for SnakerNatives");
    }

    /**
     * Forces the current windows system to "blue screen" by calling an internal debug crash function provided in <a href="https://en.wikipedia.org/wiki/Microsoft_Windows_library_files#NTDLL.DLL">NtDll.dll</a>
     *
     * @throws UnsatisfiedLinkError If an error occurs during execution
     **/
    public native void forceCrashWindows();

    /**
     * Assigns admin privileges without needing permission from the current user/super user
     *
     * @return The result of the JNI call
     * @throws UnsatisfiedLinkError If an error occurs during execution
     **/
    public native int assignElevationNoPrompt();

    /**
     * Raises an internal error on the current windows system
     * <p>
     * {@link #assignElevationNoPrompt()} must be called prior for this to do anything as admin privileges/elevation is required
     *
     * @return The result of the JNI call
     * @throws UnsatisfiedLinkError If an error occurs during execution
     **/
    public native int raiseInternalError();
}
