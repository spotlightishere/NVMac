//
//  PCIHorrorApp.swift
//  PCIHorror
//
//  Created by Spotlight Deveaux on 2025-06-03.
//

import Foundation
import SwiftUI
import SystemExtensions

class RequestDelegate: NSObject, OSSystemExtensionRequestDelegate {
    func request(_: OSSystemExtensionRequest, actionForReplacingExtension _: OSSystemExtensionProperties, withExtension _: OSSystemExtensionProperties) -> OSSystemExtensionRequest.ReplacementAction {
        .replace
    }

    func requestNeedsUserApproval(_: OSSystemExtensionRequest) {
        print("requested user")
    }

    func request(_: OSSystemExtensionRequest, didFinishWithResult _: OSSystemExtensionRequest.Result) {
        print("yippee! installed")
    }

    func request(_: OSSystemExtensionRequest, didFailWithError error: any Error) {
        fatalError("oops: \(error)")
    }
}

@main
struct PCIHorrorApp: App {
    let delegate = RequestDelegate()

    init() {
        let request = OSSystemExtensionRequest.activationRequest(forExtensionWithIdentifier: "space.joscomputing.PCIHorror.PCIHorrorKit", queue: .main)
        request.delegate = delegate
        OSSystemExtensionManager.shared.submitRequest(request)
    }

    var body: some Scene {
        WindowGroup {}
    }
}
